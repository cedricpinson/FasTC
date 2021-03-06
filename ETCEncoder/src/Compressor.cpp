/* FasTC
 * Copyright (c) 2013 University of North Carolina at Chapel Hill.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for educational, research, and non-profit purposes, without
 * fee, and without a written agreement is hereby granted, provided that the
 * above copyright notice, this paragraph, and the following four paragraphs
 * appear in all copies.
 *
 * Permission to incorporate this software into commercial products may be
 * obtained by contacting the authors or the Office of Technology Development
 * at the University of North Carolina at Chapel Hill <otd@unc.edu>.
 *
 * This software program and documentation are copyrighted by the University of
 * North Carolina at Chapel Hill. The software program and documentation are
 * supplied "as is," without any accompanying services from the University of
 * North Carolina at Chapel Hill or the authors. The University of North
 * Carolina at Chapel Hill and the authors do not warrant that the operation of
 * the program will be uninterrupted or error-free. The end-user understands
 * that the program was developed for research purposes and is advised not to
 * rely exclusively on the program for any reason.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
 * AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF
 * THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA
 * AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 * DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY 
 * STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON
 * AN "AS IS" BASIS, AND THE UNIVERSITY  OF NORTH CAROLINA AT CHAPEL HILL AND
 * THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, 
 * ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Please send all BUG REPORTS to <pavel@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Pavel Krajcevski
 * Dept of Computer Science
 * 201 S Columbia St
 * Frederick P. Brooks, Jr. Computer Science Bldg
 * Chapel Hill, NC 27599-3175
 * USA
 * 
 * <http://gamma.cs.unc.edu/FasTC/>
 */

#include "rg_etc1.h"
#include "ETCCompressor.h"
#include <algorithm>
#include <cstring>

namespace ETCC {

  void Compress_RG(const FasTC::CompressionJob &cj) {

    rg_etc1::etc1_pack_params params;
    params.m_quality = rg_etc1::cLowQuality;
    rg_etc1::pack_etc1_block_init();

    uint32 kBlockSz = GetBlockSize(FasTC::eCompressionFormat_ETC1);
    const uint32 startBlock = cj.CoordsToBlockIdx(cj.XStart(), cj.YStart());
    uint8 *outBuf = cj.OutBuf() + startBlock * kBlockSz;

    const uint32 endY = std::min(cj.YEnd(), cj.Height() - 4);
    uint32 startX = cj.XStart();
    for(uint32 j = cj.YStart(); j <= endY; j += 4) {
      const uint32 endX = j == cj.YEnd()? cj.XEnd() : cj.Width();
      for(uint32 i = startX; i < endX; i += 4) {

        uint32 pixels[16];
        const uint32 *inPixels = reinterpret_cast<const uint32 *>(cj.InBuf());
        memcpy(pixels, inPixels + j*cj.Width() + i, 4 * sizeof(uint32));
        memcpy(pixels + 4, inPixels + (j+1)*cj.Width() + i, 4 * sizeof(uint32));
        memcpy(pixels + 8, inPixels + (j+2)*cj.Width() + i, 4 * sizeof(uint32));
        memcpy(pixels + 12, inPixels + (j+3)*cj.Width() + i, 4 * sizeof(uint32));

        pack_etc1_block(outBuf, pixels, params);
        outBuf += kBlockSz;
      }
      startX = 0;
    }
  }
}  // namespace PVRTCC
