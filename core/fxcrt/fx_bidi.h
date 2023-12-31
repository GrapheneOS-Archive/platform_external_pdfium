// Copyright 2014 The PDFium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FXCRT_FX_BIDI_H_
#define CORE_FXCRT_FX_BIDI_H_

#include <stdint.h>

#include <vector>

#include "core/fxcrt/widestring.h"

// Processes characters and group them into segments based on text direction.
class CFX_BidiChar {
 public:
  enum class Direction { kNeutral, kLeft, kRight, kLeftWeak };
  struct Segment {
    int32_t start;        // Start position.
    int32_t count;        // Character count.
    Direction direction;  // Segment direction.
  };

  CFX_BidiChar();

  // Append a character and classify it as left, left-weak, right, or neutral.
  // Returns true if the character has a different direction than the
  // existing direction to indicate there is a segment to process.
  bool AppendChar(wchar_t wch);

  // Call this after the last character has been appended. AppendChar()
  // must not be called after this.
  // Returns true if there is still a segment to process.
  bool EndChar();

  // Call after a change in direction is indicated by the above to get
  // information about the segment to process.
  const Segment& GetSegmentInfo() const { return m_LastSegment; }

 private:
  void StartNewSegment(CFX_BidiChar::Direction direction);

  Segment m_CurrentSegment;
  Segment m_LastSegment;
};

class CFX_BidiString {
 public:
  using const_iterator = std::vector<CFX_BidiChar::Segment>::const_iterator;

  explicit CFX_BidiString(const WideString& str);
  ~CFX_BidiString();

  // Overall direction is always LEFT or RIGHT, never NEUTRAL.
  CFX_BidiChar::Direction OverallDirection() const;

  // Force the overall direction to be R2L regardless of what was detected.
  void SetOverallDirectionRight();

  const_iterator begin() const { return m_Order.begin(); }
  const_iterator end() const { return m_Order.end(); }

 private:
  const WideString& m_Str;
  std::vector<CFX_BidiChar::Segment> m_Order;
  CFX_BidiChar::Direction m_eOverallDirection = CFX_BidiChar::Direction::kLeft;
};

#endif  // CORE_FXCRT_FX_BIDI_H_
